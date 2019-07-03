﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CGEditor_WinForms.CustomPropertyEditors
{
    class ObservableCollections
    {
        public static List<string> GetAligns()
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

        public static List<string> GetMoveTypes()
        {
            List<string> col = new List<string>();
            col.Add("linear");
            col.Add("accel-both");
            col.Add("accel-start");
            col.Add("accel-stop");
            return col;
        }
        public static List<string> GetScaleTypes()
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

        public static List<string> GetInterlaceTypes()
        {
            List<string> col = new List<string>();
            col.Add("auto");
            col.Add("top");
            col.Add("bottom");
            col.Add("progressive");
            return col;
        }

        public static List<string> GetPlayModes()
        {
            List<string> col = new List<string>();
            col.Add("loop");
            col.Add("onetime");
            col.Add("onetime-hide");
            return col;
        }

        public static List<string> GetFlipModes()
        {
            List<string> col = new List<string>();
            col.Add("");
            col.Add("X");
            col.Add("Y");
            col.Add("XY");
            return col;
        }

        public static List<string> GetDefaultFontSizes()
        {
            List<string> col = new List<string>();
            col.Add("8");
            col.Add("9");
            col.Add("10");
            col.Add("11");
            col.Add("12");
            col.Add("14");
            col.Add("16");
            col.Add("18");
            col.Add("20");
            col.Add("22");
            col.Add("24");
            col.Add("26");
            col.Add("28");
            col.Add("36");
            col.Add("48");
            col.Add("72");
            return col;
        }

        public static List<string> GetShapeTypes()
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

        public static List<string> GetGradientTypes()
        {
            List<string> col = new List<string>();
            col.Add("Horizontal");
            col.Add("Vertical");
            col.Add("Radial");
            col.Add("Follow");
            col.Add("Diagonal");
            return col;
        }
    }
}
