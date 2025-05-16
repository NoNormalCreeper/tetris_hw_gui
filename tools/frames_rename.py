"""
一次性脚本，把 mainwindow.ui 中所有的 QFrame 控件的名称改为frame_1, frame_2, frame_3...
"""

import xml.etree.ElementTree as ET
from pathlib import Path
#
src_path = Path(__file__).parent.parent / "src" / "mainwindow.ui"
dst_path = Path(__file__).parent.parent / "src" / "mainwindow_modified.ui"
#
# tree = ET.parse(src_path)
# root = tree.getroot()
# ui = root.find("ui")
# widget = root.find("widget")
#
# # 找到 horizontalLayout_1
# for layout in ui.iter("layout"):
#     if layout.attrib.get("name").startswith("horizontalLayout_"):
#         count = 1
#         for item in layout.findall("item"):
#             widget = item.find("widget")
#             if widget is not None and widget.attrib.get("class") == "QFrame":
#                 widget.set("name", f"frame_{count}")
#                 count += 1
#
# tree.write(dst_path, encoding="utf-8", xml_declaration=True)

# import xml.etree.ElementTree as ET

tree = ET.parse(src_path)
root = tree.getroot()

i = 1

for layout in root.iter('layout'):
    if layout.attrib.get('name').startswith('horizontalLayout_'):
        for item in layout.findall('item'):
            widget = item.find('widget')
            if widget is not None and widget.attrib.get('class') == 'QFrame':
                original_name = widget.attrib.get('name')
                widget.attrib['name'] = f'frame_{i}'
                print(f"Renamed {original_name} to frame_{i}")
                i += 1

tree.write(dst_path, encoding='utf-8', xml_declaration=True)

# tree = ET.parse(src_path)
# root = tree.getroot()
#
# # Find all layout elements in the XML
# for layout in root.iter("layout"):
#     print(layout.attrib)