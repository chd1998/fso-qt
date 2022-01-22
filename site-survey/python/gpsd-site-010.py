from gpsdclient import GPSDClient
import time
from datetime import datetime
from datetime import timezone
from datetime import timedelta

client = GPSDClient(host="127.0.0.1",port="7070")

# get your data as json strings:
#for result in client.json_stream():
#    print(result)
print("==============================================")
# or as python dicts (optionally convert time information to `datetime` objects)
for result in client.dict_stream(convert_datetime=True):
    if result["class"] == "TPV":
      latitute=result.get("lat", "n/a")
      longitute=result.get("lon", "n/a")
      altitute=result.get("alt", "n/a")
      utc_now=result.get("time", "n/a")
      cst_now=utc_now.astimezone(timezone(timedelta(hours=-8)))
      print("Latitude: %07s" % latitute)
      print("Longitude: %07s" % longitute)
      print("Altitude: %08s" % altitute)
      print("Date: %s" % cst_now.date())
      print("Time: %s" % cst_now.time())
      print("==============================================")
      time.sleep(1)
        
