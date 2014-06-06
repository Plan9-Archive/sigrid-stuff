#!/bin/rc

country=$1
region=$2
city=$3
if($#* = 2){
	city=$region
	region=None
}
if(test $#* -lt 2){
	echo usage: iss country [region] city
	exit usage
}

hget 'http://spotthestation.nasa.gov/sightings/view.cfm?country='^$country^'&region='^$region^'&city='^$city | awk '
	BEGIN{
		head=0
		a[1] = 20
		a[2] = 9
		a[3] = 12
		a[4] = 14
		a[5] = 14
	}
	/<[\/]t[hd]>/ && !/Share/ {
		gsub(/[ \t]*<[^>]*>[ \t]*/, "")
		sub(/&deg;/, "°")
		if($0 != ""){
			if(head > 3)
				a[3] = 13
			head++
			printf "%"a[head]"s", $0
		}
	}
	/<tr>/ && head {
		print ""
		head = 0
	}
	END {
		print ""
	}'
