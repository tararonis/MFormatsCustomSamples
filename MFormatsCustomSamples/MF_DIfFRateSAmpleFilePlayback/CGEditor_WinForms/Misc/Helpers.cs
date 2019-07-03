using System;
using System.Drawing;
using System.Linq;

namespace CGEditor.Misc
{
    static class Helpers
    {
        public static Color ParseCGColor(string strCGColor)
        {
            Color res = Color.Transparent;
            if (strCGColor != null && strCGColor != string.Empty)
            {
                string strColor = strCGColor.Trim();
                string strAlpha = "";
                int nAlpha = 0;

                if (strCGColor.Contains('('))
                {
                    string[] split = strCGColor.Split('(');
                    strColor = split[0];
                    strAlpha = split[1].Replace(")", "");
                }

                if (strColor.ToLower() == "ml")
                {
                    res = Color.FromArgb(0,0,88,143);
                }
                else
                    res = (Color)ColorTranslator.FromHtml(strColor);
                if (strAlpha != String.Empty)
                {
                    bool bSucceded = Int32.TryParse(strAlpha, out nAlpha);
                    if (bSucceded)
                        res = Color.FromArgb((Byte)nAlpha, res.R, res.G, res.B);
                }
            }
            return res;
        }

        public static Color[] parseCGradient(string strCGGradient, out bool isVertical)
        {
            Color[] res = null;
            isVertical = false;
            if (strCGGradient != null && strCGGradient != string.Empty)
            {
                if (strCGGradient.Contains("/"))
                    isVertical = true;

                string[] strGradients = strCGGradient.Split('-');

                if (strGradients.Length < 2)
                    strGradients = strCGGradient.Split('+');

                if (strGradients.Length < 2)
                    strGradients = strCGGradient.Split('*');

                if (strGradients.Length < 2)
                    strGradients = strCGGradient.Split('/');

                res = new Color[strGradients.Length];
                for (int i = 0; i < strGradients.Length; i++)
                {
                    res[i] = ParseCGColor(strGradients[i]);
                }
            }
            return res;
        }

        public static bool isStringTrue(string strTarget)
        {
            if (strTarget.ToLower() == "true" || strTarget.ToLower() == "yes" || strTarget.ToLower() == "1")
                return true;
            else
                return false;
        }
    }
}
