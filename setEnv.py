import os
a=os.popen('git log --pretty=format:“%s” -1')
commit_msg = a.readline()

update_module=commit_msg.split(":")

if len(update_module)>1:
    for key in update_module[0].split(","):
        print(key)