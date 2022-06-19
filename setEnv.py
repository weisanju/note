import os
a=os.popen('git log --pretty=format:%s -1')
commit_msg = a.readline()

update_module=commit_msg.split(":")
with open("setEnv.sh", mode='w', encoding='utf-8') as file_obj:
    if len(update_module)>1:
        for key in update_module[0].split(","):
            file_obj.write("{}=1\n".format(key))