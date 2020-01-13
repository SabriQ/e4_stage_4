#from func_train import stage_3
# from stage_1 import stage_1
# from stage_3 import stage_3_train
# from stage_3 import stage_3_test
from stage_4 import stage_4
from info import send_wechat_by_serverchan
import sys
if __name__ == "__main__":
    send_wechat = send_wechat_by_serverchan()
    stage_4(serial_ports=['COM18'],mouse_id=sys.argv[1],note="train",description="no_description"
            ,video_record = False,according_to="Trial",Time=1200,Trial=60
            ,data_dir=r"E:\linear_track")
    send_wechat(f"{sys.argv[1]} finish training","nothing")
