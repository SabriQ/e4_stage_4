from linear_track_funcs import *
from sys_camera import *
def stage_4(serial_ports=[r'/dev/ttyUSB0'],mouse_id=r"192137",note="train",description="no_description",video_record = False,
             according_to="Trial",Time=1200,Trial=60,data_dir=r"C:\Users\Sabri\Desktop\test"):
    '''
    stage3 is shuttle run ...
    '''
    #检查端口是否可用
    check_ports(serial_ports)
    ser_ctrl = serial.Serial(serial_ports[0],baudrate=9600,timeout=0.1)
    countdown(3)
    #设置数据存放路径 年月日；设置log文件，视频文件的名字；context_orders
    current_time = time.strftime("%Y%m%d-%H%M%S", time.localtime())
    data_dir = os.path.join(data_dir,current_time)
    if not os.path.exists(data_dir):
        os.makedirs(data_dir)
        print(f"{data_dir} is created")
#    if not os.path.exists(data_dir):
#        print("path is wrong")
#        sys.exit()
    log_name = os.path.join(data_dir,current_time+"-"+mouse_id+"-"+note+'_log.csv')
    video_name = os.path.join(data_dir,current_time+"-"+mouse_id+"-"+note+'.mp4')

    #开始实验
    #开始视频录制
    if video_record:
        input("请按Enter开始实验（倒计时3s之后开启，摄像头会率先启动）：")
        video = video_recording(video_name)
        print(f'{os.path.basename(video_name)} is recording')
        countdown(3)
    else:
        input("请按Enter开始实验:")
        #video = video_online_play()
    #在log文件中写入title
    with open(log_name, 'w',newline="",encoding='utf-8') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["mouse_id",mouse_id])
        writer.writerow(["stage2 learn to lick and alternate licking"])
        writer.writerow(["description",description])
        writer.writerow(["Trial_Num","led_stat","left_choice","right_Choice","Choice_Class","A_NosePoke","A_Choice","P_NosePoke","P_Choice"])
    print(["Trial_Num","Choice","left_choice","right_Choice","Choice_Class"])
    
    video_start_time = time.time()
    
    Trial_Num=[];led_stat=[];left_choice=[];right_choice;Choice_Class=[];A_NosePoke=[];A_Choice=[];P_NosePoke=[];P_Choice=[];
    show_info = "Ready "
    while True:
        info = ser_ctrl.readline().decode("utf-8").strip().split(" ")# waiting for 0.1s
        time_elapse = time.time()-video_start_time
        if len(info)>1:
            show_info = ''.join([i for i in info])
            if "Stat1:" in info:
                P_NosePoke.append(time_elapse);            
            if "Stat2:" in info:
                P_ContextEnter.append(time_elapse)
                if "choice_l" and "correct" in info or "choice_r" and "wrong" in info:
                    led_stat.append("flash")
                elif "choice_r" and "correct" in info or "choice_l" and "wrong" in info:
                    led_stat.append("contiuous_on")
                else:
                    led_stat.append("terminated")
            if "Sum:" in info and info[1] !=0:
                Trial_Num.append(info[1])
                left_choice.append(info[2])
                right_Choice.append(info[3])
                Choice_Class.append(info[4])
                A_NosePoke.append(info[5])
                A_Choice.append(info[7])
                A_ContextREnter.append(info[8])
                A_ContextRExit.append(info[9])
                
                row=[Trial_Num[-1],led_stat[-1],left_choice[-1],right_choice[-1],Choice_Class[-1],A_NosePoke[-1],A_Choice[-1],P_NosePoke[-1],P_Choice[-1]]
                with open(log_name,"a",newline="\n",encoding='utf-8') as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow(row)
                print(row[0:5])
        #时间进度输出
            if "Sum" in show_info:
                show_info = "Ready "
        print(f"\r{show_info}".ljust(25),f"time elapses {round(time_elapse,1)}s  ",end="")
        #sys.stdout.write("time elapses %.1fs"%(time_elapse))
        #sys.stdout.write("\r")
        #another situation: for certain number of trials
        if according_to == "Time":
            if time_elapse >=Time:
                if video_record:
                    time.sleep(1)
                    video.communicate('q')
                break
        elif according_to =="Trial": # trial <= 90 
            if len(info)>2:
                if info[1]==str(Trial):
                    if video_record:
                        time.sleep(1)
                        video.communicate('q')
                    break
        else:
            print("How do you decide to count down your experiments, 'Time'or'Trial'?")
            sys.exit()
    ser_ctrl.close()
    print(f"training log is saved in {os.path.basename(log_name)}")

