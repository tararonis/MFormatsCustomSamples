

#region Using directives

using System;

#endregion

namespace CGEditor_WinForms.CustomPropertyEditors
{
    public class ColorChangedEventArgs : EventArgs
    {
        public ColorChangedEventArgs(ColorHandler.ARGB argb, ColorHandler.HSV HSV)
        {
            ARGB = argb;
            this.HSV = HSV;
        }

        public ColorHandler.ARGB ARGB { get; private set; }

        public ColorHandler.HSV HSV { get; private set; }
    }
}