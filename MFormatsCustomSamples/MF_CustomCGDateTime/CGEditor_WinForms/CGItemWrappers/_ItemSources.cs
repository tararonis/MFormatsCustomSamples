using System.Collections.Generic;

namespace CGEditor_WinForms.CGItemWrappers
{
    public class AlignItemsSource : object
    {
        public object GetValues()
        {
            List<string> col = new List<string>();
            col.Add("center");
            col.Add("left");
            col.Add("right");
            //col.Add("!!! right-left");
            col.Add("top");
            col.Add("top-left");
            col.Add("top-right");
            //col.Add("!!! top-right-left");
            col.Add("bottom");
            col.Add("bottom-left");
            col.Add("bottom-right");
            return col;
        }
    }

    public class FontStylesItemsSource : object
    {
        public object GetValues()
        {
            List<string> col = new List<string>();
            col.Add("uppercase");
            col.Add("lowercase");
            return col;
        }
    }

    public class YesNoItemsSource : object
    {
        public object GetValues()
        {
            List<string> col = new List<string>();
            col.Add("yes");
            col.Add("no");
            return col;
        }
    }

    public class MoveTypeItemsSource : object
    {
        public object GetValues()
        {
            List<string> col = new List<string>();
            col.Add("linear");
            col.Add("accel-both");
            col.Add("accel-start");
            col.Add("accel-stop");
            return col;
        }
    }

    public class InterlaceItemsSource : object
    {
        public object GetValues()
        {
            List<string> col = new List<string>();
            col.Add("auto");
            col.Add("top");
            col.Add("bottom");
            col.Add("progressive");
            return col;
        }
    }

    public class ScaleItemsSource : object
    {
        public object GetValues()
        {
            List<string> col = new List<string>();
            col.Add("fit-ar");
            col.Add("exact-fit");
            col.Add("no-scale");
            col.Add("shrink-to-fit");
            col.Add("shrink-to-fit-ar");
            col.Add("cover-ar");
            col.Add("text-scale");
            return col;
        }
    }

    public class PlayModeItemsSource : object
    {
        public object GetValues()
        {
            List<string> col = new List<string>();
            col.Add("loop");
            col.Add("onetime");
            col.Add("onetime-hide");
            return col;
        }
    }

    public class TrueFalseItemsSource : object
    {
        public object GetValues()
        {
            List<string> col = new List<string>();
            col.Add("true");
            col.Add("false");
            return col;
        }
    }

    public class ShapeTypeSource : object
    {
        public object GetValues()
        {
            List<string> col = new List<string>();
            col.Add("rect");
            col.Add("block-1");
            col.Add("block-2");
            col.Add("block-3");
            col.Add("block-4");
            col.Add("block-5");
            col.Add("block-6");
            col.Add("circle");
            col.Add("ellipse");
            col.Add("triangle");
            col.Add("polygon");
            col.Add("star");
            return col;
        }
    }
}
