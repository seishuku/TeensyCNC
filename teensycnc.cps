description = "TeensyCNC";
vendor = "Matt Williams";
vendorUrl = "nitrogl@gmail.com";
legal = "Copyright (C) 2016 by Matt Williams";
certificationLevel = 2;
minimumRevision = 24000;

longDescription = "Post for TeensyCNC";

extension = "nc";
setCodePage("ascii");

capabilities=CAPABILITY_MILLING|CAPABILITY_JET;

tolerance=spatial(0.002, MM);

minimumChordLength=spatial(0.01, MM);
minimumCircularRadius=spatial(0.01, MM);
maximumCircularRadius=spatial(1000, MM);
minimumCircularSweep=toRad(0.01);
maximumCircularSweep=toRad(180);
allowHelicalMoves=true;
allowedCircularPlanes=undefined; // allow any circular motion

// user-defined properties
properties=
{
};

var gFormat=createFormat({prefix:"G", decimals:0});
var mFormat=createFormat({prefix:"M", decimals:0});

var xyzFormat=createFormat({decimals:(unit==MM?3:4), forceDecimal:true});
var feedFormat=createFormat({decimals:(unit==MM?1:2)});

var xOutput=createVariable({prefix:"X", force:true}, xyzFormat);
var yOutput=createVariable({prefix:"Y", force:true}, xyzFormat);
var feedOutput=createVariable({prefix:"F"}, feedFormat);

var iOutput=createReferenceVariable({prefix:"I", force:true}, xyzFormat);
var jOutput=createReferenceVariable({prefix:"J", force:true}, xyzFormat);

var gMotionModal=createModal({force:true}, gFormat); // modal group 1 // G0-G3, ...
var gAbsIncModal=createModal({}, gFormat); // modal group 3 // G90-91
var gUnitModal=createModal({}, gFormat); // modal group 6 // G20-22

function writeBlock()
{
	writeWords(arguments);
}

function onOpen()
{
	// absolute coordinates and feed per min
	writeBlock(gAbsIncModal.format(90));

	switch(unit)
	{
		case IN:
			writeBlock(gUnitModal.format(20));
			break;

		case MM:
			writeBlock(gUnitModal.format(21));
			break;
	}
}

function onComment(message)
{
}

function forceXYZ()
{
	xOutput.reset();
	yOutput.reset();
}

function forceAny()
{
	forceXYZ();
	feedOutput.reset();
}

function onSection()
{
	forceXYZ();
	forceAny();
}

function onRapid(_x, _y, _z)
{
//    writeBlock(mFormat.format(8));

	writeBlock(gMotionModal.format(0), xOutput.format(_x), yOutput.format(_y));
	feedOutput.reset();

//    writeBlock(mFormat.format(7));
}

function onLinear(_x, _y, _z, feed)
{
	xOutput.reset();
	yOutput.reset();

	writeBlock(gMotionModal.format(1), xOutput.format(_x), yOutput.format(_y), feedOutput.format(feed));
}

function onCircular(clockwise, cx, cy, cz, x, y, z, feed)
{
	var start=getCurrentPosition();

	if(isFullCircle())
	{
		if(isHelical())
		{
			linearize(tolerance);
			return;
		}

		switch(getCircularPlane())
		{
			case PLANE_XY:
				writeBlock(gMotionModal.format(clockwise?2:3), xOutput.format(x), iOutput.format(cx-start.x, 0), jOutput.format(cy-start.y, 0), feedOutput.format(feed));
				break;

			default:
				linearize(tolerance);
		}
	}
	else
	{
		switch(getCircularPlane())
		{
			case PLANE_XY:
				writeBlock(gMotionModal.format(clockwise?2:3), xOutput.format(x), yOutput.format(y), iOutput.format(cx-start.x, 0), jOutput.format(cy-start.y, 0), feedOutput.format(feed));
				break;

			default:
				linearize(tolerance);
		}
	}
}

function onPower(enable)
{
    if(enable)
        writeBlock(mFormat.format(7));
    else
        writeBlock(mFormat.format(8));
}

var mapCommand=
{
	COMMAND_STOP:0,
	COMMAND_OPTIONAL_STOP:1
};

function onCommand(command)
{
	switch(command)
	{
        case COMMAND_POWER_ON:
            return;
        case COMMAND_POWER_OFF:
            return;
	}

	var mcode=mapCommand[getCommandStringId(command)];

	if(mcode!=undefined)
		writeBlock(mFormat.format(mcode));
	else
		onUnsupportedCommand(command);
}

function onSectionEnd()
{
	forceAny();
	writeBlock(mFormat.format(8)); // head up
}

function onClose()
{
//	writeBlock(mFormat.format(8)); // head up
	writeBlock(gFormat.format(28)); // home XYZ
	writeBlock(gFormat.format(2)); // end
}
