package;

#if !macro
import js.Browser;
import coroutine.Routine;
import coroutine.Routine.RoutineInstruction.WaitDelay;
import coroutine.CoroutineRunner;
#end

#if macro
import haxe.io.Path;
import haxe.macro.Context;
import haxe.macro.Expr;
import haxe.macro.Type;
#end

class Macros {
    public static macro function getFileContent(filePath:String):ExprOf<String> {
        if(sys.FileSystem.exists(filePath)) {
            var fileContent:String = sys.io.File.getContent(filePath);
            return macro $v {fileContent};
        }  else {
            return macro null;
        }
    }
}

#if !macro

class Point {
    public var x:Int;
    public var y:Int;

    public function new(x, y) {
        this.x = x;
        this.y = y;
    }
}

class VentLine {
    public var a:Point;
    public var b:Point;

    public function new(a, b) {
        this.a = a;
        this.b = b;
    }

    public function isAxisAligned() {
        return a.x == b.x || a.y == b.y;
    }

    public function getStep():Point {
        var dx = b.x - a.x;
        var dy = b.y - a.y;

        if(dx != 0) {
            dx = Std.int(dx / Math.abs(dx));
        }

        if(dy != 0) {
            dy = Std.int(dy / Math.abs(dy));
        }

        return new Point(dx, dy);
    }
}


class Main {
    var width:Int = 1024;
    var height:Int = 1024;
    var context2d:js.html.CanvasRenderingContext2D;
    var coroutineRunner = new CoroutineRunner();

    function delay():Routine {

        trace(Macros.getFileContent("input.txt"));

        trace("coucou");
        @yield return WaitDelay(2);

        trace("beuh");
        @yield return WaitDelay(2);

        trace("yak");
    }

    public function new() {
        var canvas:js.html.CanvasElement = cast js.Browser.document.getElementsByTagName("canvas")[0];
        canvas.style.width = width + "px";
        canvas.style.height = height + "px";
        canvas.width = width;
        canvas.height = height;
        context2d = canvas.getContext("2d");
        update(0);
        coroutineRunner.startCoroutine(delay());
    }

    function update(time:Float) {
        var processor = CoroutineProcessor.of(coroutineRunner);
        processor.updateEnterFrame();
        processor.updateTimer(time/1000);
        processor.updateExitFrame();
        drawGrid();
        js.Browser.window.requestAnimationFrame(update);
    }

    inline function drawLine(a, b, c, d) {
        context2d.beginPath();
        context2d.moveTo(a, b);
        context2d.lineTo(c, d);
        context2d.stroke();
    }

    inline function drawCircle(x, y, r) {
        context2d.beginPath();
        context2d.arc(x, y, r, 0, 2 * Math.PI);
        context2d.fill();
    }

    inline function drawGrid() {
        var tileSize = 64;
        context2d.fillStyle  = "#eee";
        context2d.fillRect(0, 0, width, height);
        context2d.fillStyle = "#833";
    }

    static function main() {
        new Main();
    }
}
#end
