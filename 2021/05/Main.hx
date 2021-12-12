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
    public
    static macro function getFileContent(filePath : String) : ExprOf<String> {
        if(sys.FileSystem.exists(filePath)) {
            var fileContent : String = sys.io.File.getContent(filePath);
            return macro $v {fileContent};
        } else {
            return macro null;
        }
    }
}

#if !macro

class Point {
    public var x : Int;
    public var y : Int;

    public function new(x, y) {
        this.x = x;
        this.y = y;
    }

    static public function parse(input : String) {
        var parts = input.split(",");
        return new Point(Std.parseInt(parts[0]), Std.parseInt(parts[1]));
    }

    static public function isEqual(a : Point, b : Point) {
        return a.x == b.x && a.y == b.y;
    }
}

class VentLine {
    public var a : Point;
    public var b : Point;

    public function new(a, b) {
        this.a = a;
        this.b = b;
    }

    public function isAxisAligned() {
        return a.x == b.x || a.y == b.y;
    }

    public function getStep() : Point {
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

class Context {
    public var ventLines = new Array<VentLine>();
    public var grid = new Map<Int, Int>();
    public var intersections = new Array<Point>();

    public function increase(point : Point) {
        var index = point.x + point.y * 5000;
        var val = grid.get(index);

        if(val == null) {
            grid.set(index, 0);
            val = 0;
        }

        grid[index] = val + 1;
    }
    public function new() {};
}

class Main {
    var width : Int = 1024;
    var height : Int = 1024;
    var context2d : js.html.CanvasRenderingContext2D;
    var coroutineRunner = new CoroutineRunner();
    var context = new Context();

    function solve() : Routine {
        var lines = Macros.getFileContent("input.txt").split("\n");
        lines.pop();

        for(i in 0 ...3) {
            trace(3 - i);
            @yield return WaitDelay(1);
        }


        var i = 0;

        for(line in lines) {
            var parts = line.split(" -> ");
            var ventLine = new VentLine(Point.parse(parts[0]), Point.parse(parts[1]));
            context.ventLines.push(ventLine);

            if(i % 10 == 0) {
                @yield return WaitDelay(0.01);
            }

            ++i;
        }

        for(line in context.ventLines) {
            var p = line.a;
            var step = line.getStep();

            do {
                context.increase(p);
                p.x += step.x;
                p.y += step.y;
            } while(!Point.isEqual(p, line.b));

            context.increase(p);
        }

        var intersections = new Array<Point>();

        for(k in context.grid.keys()) {
            var v = context.grid[k];

            if(v >= 2) {
                intersections.push(new Point(k%5000, Std.int(k/5000)));
            }
        }

        while(intersections.length > 0) {
            for(j in 0...200) {
                var item = intersections.pop();

                if(item == null) { break; }

                context.intersections.push(item);
            }

            @yield return WaitDelay(0.01);
        }

        /* trace(r); */
        @yield return WaitDelay(0.001);
    }

    public function new() {
        var canvas : js.html.CanvasElement = cast js.Browser.document.getElementsByTagName("canvas")[0];
        canvas.style.width = width + "px";
        canvas.style.height = height + "px";
        canvas.width = width;
        canvas.height = height;
        context2d = canvas.getContext("2d");
        context2d.lineWidth = 2;
        update(0);
        coroutineRunner.startCoroutine(solve());
    }

    function update(time : Float) {
        var processor = CoroutineProcessor.of(coroutineRunner);
        processor.updateEnterFrame();
        processor.updateTimer(time / 1000);
        processor.updateExitFrame();
        render();
        js.Browser.window.requestAnimationFrame(update);
    }

    inline function drawLine(a, b) {
        context2d.beginPath();
        context2d.moveTo(a.x + 0.5, a.y + 0.5);
        context2d.lineTo(b.x + 0.5, b.y + 0.5);
        context2d.stroke();
    }

    inline function drawCircle(p, r) {
        context2d.beginPath();
        context2d.arc(p.x, p.y, r, 0, 2 * Math.PI);
        context2d.fill();
    }

    inline function render() {
        context2d.strokeStyle = "#666";

        for(line in context.ventLines) {
            drawLine(line.a, line.b);
        }

        context2d.strokeStyle = "DA76";
        context2d.fillStyle = "#D44";
        context2d.moveTo(0, 0);

        for(point in context.intersections) {
            drawCircle(point, 5);
        }
    }

    static function main() {
        new Main();
    }
}
#end
