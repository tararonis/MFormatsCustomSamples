using System;
using System.Drawing;
using System.Linq;
using CGEditor_WinForms.CustomPropertyEditors;

namespace CGEditor.CGItemWrappers
{
    public class WrapperHelpers
    {
        public static Color ParseCGColor(string cgColor)
        {
            Color res = Color.Transparent;
            if (!string.IsNullOrEmpty(cgColor))
            {
                string strColor = cgColor.Trim();
                string strAlpha = "";

                if (cgColor.Contains('('))
                {
                    string[] split = cgColor.Split('(');
                    strColor = split[0];
                    strAlpha = split[1].Replace(")", "");
                }

                if (strColor.ToLower() == "ml")
                {
                    res = Color.FromArgb(0, 0, 88, 143);
                }
                else
                {
                    try
                    {
                        res = Color.FromName(strColor); 
                    }
                    catch (Exception)
                    {
                        if (!strColor.Contains('#'))
                            strColor = strColor.Insert(0, "#");
                        res = ColorTranslator.FromHtml(strColor);
                    }
                }
                if (strAlpha != String.Empty)
                {
                    int nAlpha;
                    bool bSucceded = Int32.TryParse(strAlpha, out nAlpha);
                    if (bSucceded)
                        res = Color.FromArgb((Byte) nAlpha, res.R, res.G, res.B);
                    //res.A = (Byte)nAlpha;
                }
            }
            else if (cgColor != null && cgColor == string.Empty)
            {
                res = Color.White;
            }
            return res;
        }

        public static Color[] parseCGradient(string cgGradient, out string gradientType)
        {
            Color[] res = null;
            gradientType = "Horizontal";
            if (cgGradient != null)
            {
                cgGradient = cgGradient.Trim();

                string[] strGradients = null;
                if (cgGradient.Contains('-'))
                {
                    strGradients = cgGradient.Split('-');
                    gradientType = "Horizontal";
                }
                else if (cgGradient.Contains('/'))
                {
                    strGradients = cgGradient.Split('/');
                    gradientType = "Vertical";
                }
                else if (cgGradient.Contains('*'))
                {
                    strGradients = cgGradient.Split('*');
                    gradientType = "Radial";
                }
                else if (cgGradient.Contains('|'))
                {
                    strGradients = cgGradient.Split('|');
                    gradientType = "Follow";
                }
                else if (cgGradient.Contains('+'))
                {
                    strGradients = cgGradient.Split('+');
                    gradientType = "Diagonal";
                }

                if (strGradients == null)
                    strGradients = new string[] { cgGradient };


                res = new Color[strGradients.Length];
                for (int i = 0; i < strGradients.Length; i++)
                {
                    res[i] = ParseCGColor(strGradients[i]);
                }
            }
            return res;
        }

        public static Color[] parseCGradient(string cgGradient, out bool isVertical)
        {
            Color[] res = null;
            isVertical = false;
            if (!string.IsNullOrEmpty(cgGradient))
            {
                if (cgGradient.Contains("/"))
                    isVertical = true;

                string[] strGradients = cgGradient.Split('-');

                if (strGradients.Length < 2)
                    strGradients = cgGradient.Split('+');

                if (strGradients.Length < 2)
                    strGradients = cgGradient.Split('*');

                if (strGradients.Length < 2)
                    strGradients = cgGradient.Split('/');

                res = new Color[strGradients.Length];
                for (int i = 0; i < strGradients.Length; i++)
                {
                    res[i] = ParseCGColor(strGradients[i]);
                }
            }
            return res;
        }

        public static string ComposeCGGradient(Color[] colors, string gradientType)
        {
            string strRes = string.Empty;
            //Check for valid gradient type
            if (gradientType != "Horizontal" && gradientType != "Vertical" && gradientType != "Radial"
                && gradientType != "Follow" && gradientType != "Diagonal")
                gradientType = null;

            if (colors.Length > 0 && gradientType != null)
            {
                for (int i = 0; i < colors.Length; i++)
                {

                    string colorCode = SimpleColorEditor.ColorToHexString(colors[i]);
                    strRes += colorCode;
                    if (i < colors.Length - 1)
                    {
                        if (gradientType == "Horizontal")
                            strRes += "-";
                        if (gradientType == "Vertical")
                            strRes += "/";
                        if (gradientType == "Radial")
                            strRes += "*";
                        if (gradientType == "Follow")
                            strRes += "|";
                        if (gradientType == "Diagonal")
                            strRes += "+";
                    }
                }
            }
            return strRes;
        }

        public static bool isStringTrue(string target)
        {
            if (target.ToLower() == "true" || target.ToLower() == "yes" || target.ToLower() == "1")
                return true;
            else
                return false;
        }
    }
}
