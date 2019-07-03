namespace CGEditor.CGItemWrappers
{
    public class CGFlashItem : CGBaseItem, IPath
    {
        public CGFlashItem(string itemId) : base(itemId) { }

        public string Path
        {
            get
            {
                return GetStringProperty("img::path");
            }
            set
            {
                SetProperty("img::path", value);
                InvokeItemChanged(this, new ItemChangedArgs("path", value));
            }
        }
    }
}
