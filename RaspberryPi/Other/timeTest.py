from datetime import datetime
import calendar

d = datetime.utcnow()
print calendar.timegm(d.utctimetuple())