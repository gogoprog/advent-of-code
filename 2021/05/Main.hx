package;

import js.Browser;
import coroutine.Routine;
import coroutine.Routine.RoutineInstruction.WaitDelay;
import coroutine.CoroutineRunner;

class Main {
    var width:Int = 1024;
    var height:Int = 1024;
    var context2d:js.html.CanvasRenderingContext2D;
    var coroutineRunner = new CoroutineRunner();

    function delay():Routine {

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
