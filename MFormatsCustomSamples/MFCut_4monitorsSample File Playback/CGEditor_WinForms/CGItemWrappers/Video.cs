using System.ComponentModel;
using CGEditor.CGItemWrappers;

namespace CGEditor_WinForms.CGItemWrappers
{
    [DisplayName("Video Item Properties")]
    public class CGVideoItem : CGBaseItem, IPath
    {

        public CGVideoItem(string itemId) : base(itemId) { }

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
