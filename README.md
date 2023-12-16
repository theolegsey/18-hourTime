# 18-hourTime
Time, that include 36 hours, 324 minutes and 324 seconds
It has pretty simple scheme. Firstly we recieve time from GPS. It send hours, minutes and seconds. Transform all time in seconds and multiply on 0.135(5.4x5.4x1.5:324) Then we take full part after dividing multiplyed seconds on 324 - that hours in 18-hour time. Reminder we again multiply on 324 and we got minutes in new time format
