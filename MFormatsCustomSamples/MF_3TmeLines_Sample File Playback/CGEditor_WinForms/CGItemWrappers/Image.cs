using System.ComponentModel;

namespace CGEditor.CGItemWrappers
{
    [DisplayName("Text Item Properties")]
    public class CGImageItem : CGBaseItem, IPath
    {

        public CGImageItem(string itemId) : base(itemId) { }

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
