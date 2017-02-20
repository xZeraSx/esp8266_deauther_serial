#include "APScan.h"

APScan::APScan(){
  
}

bool APScan::setAsyncIndex() {
    asyncIndex = results-1;
    if(asyncIndex > ApScanMaxResults) {
      asyncIndex = ApScanMaxResults-1;
    }
    return true;
    
}

int APScan::scan(){
    aps._clear();
    selected = -1;
    results = WiFi.scanNetworks();
    setAsyncIndex();
    return results;
}



bool APScan::start(){
    aps._clear();
    selected = -1;
    results = WiFi.scanNetworks();
    asyncIndex = -1;
    
    for(int i=0;i<results && i<ApScanMaxResults;i++){
      
      Mac _ap;
      _ap.set(WiFi.BSSID(i)[0],WiFi.BSSID(i)[1],WiFi.BSSID(i)[2],WiFi.BSSID(i)[3],WiFi.BSSID(i)[4],WiFi.BSSID(i)[5]);
      aps.add(_ap);
      channels[i] = WiFi.channel(i);
      rssi[i] = WiFi.RSSI(i);
      getEncryption(WiFi.encryptionType(i)).toCharArray(encryption[i],5);
      WiFi.SSID(i).toCharArray(names[i],33);
      data_getVendor(WiFi.BSSID(i)[0],WiFi.BSSID(i)[1],WiFi.BSSID(i)[2]).toCharArray(vendors[i],9);
      
    }
    return true;
}

String APScan::getEncryption(int code){
  switch (code) {
    case ENC_TYPE_NONE:
      return "none";
      break;
    case ENC_TYPE_WEP:
      return "WEP";
      break;
    case ENC_TYPE_TKIP:
      return "WPA";
      break;
    case ENC_TYPE_CCMP:
      return "WPA2";
      break;
    case ENC_TYPE_AUTO:
      return "WPA*";
      break;
    }
}

String APScan::getAPName(int num){ return names[num]; }
String APScan::getAPEncryption(int num){ return encryption[num]; }
String APScan::getAPVendor(int num){ return vendors[num]; }
String APScan::getAPMac(int num){ return aps._get(num).toString(); }
String APScan::getAPSelected(int num){
  if(selected == num) return "true";
  else return "false";  
}
int APScan::getAPRSSI(int num){ return rssi[num]; }
int APScan::getAPChannel(int num){ return channels[num]; }

Mac APScan::getTarget(){
  return aps._get(selected);
}


String APScan::getResult(int i){
  String json = "{ \"aps\":[ ";
  json += "{";
  json += "\"id\": "+(String)i+",";
  json += "\"channel\": "+(String)getAPChannel(i)+",";
  json += "\"mac\": \""+getAPMac(i)+"\",";
  json += "\"ssid\": \""+getAPName(i)+"\",";
  json += "\"rssi\": "+(String)getAPRSSI(i)+",";
  json += "\"encryption\": \""+getAPEncryption(i)+"\",";
  json += "\"vendor\": \""+getAPVendor(i)+"\",";
  json += "\"selected\": "+getAPSelected(i);
  json += "}";
  json += "] }";
  return json;
}


String APScan::getResults(){
  String json = "{ \"aps\":[ ";
  for(int i=0;i<results && i<ApScanMaxResults;i++){
    json += "{";
    json += "\"id\": "+(String)i+",";
    json += "\"channel\": "+(String)getAPChannel(i)+",";
    json += "\"mac\": \""+getAPMac(i)+"\",";
    json += "\"ssid\": \""+getAPName(i)+"\",";
    json += "\"rssi\": "+(String)getAPRSSI(i)+",";
    json += "\"encryption\": \""+getAPEncryption(i)+"\",";
    json += "\"vendor\": \""+getAPVendor(i)+"\",";
    json += "\"selected\": "+getAPSelected(i);
    json += "}";
    if((i!=results-1) && (i!=ApScanMaxResults-1)) json += ",";
  }
  json += "] }";
  return json;
}

int APScan::getResultByAPName(String apName){
  for(int i=0;i<results && i<ApScanMaxResults;i++){
    if(apName!=getAPName(i) && apName!=getAPMac(i)) continue;
    return i;
  }
  return -1;
}


int APScan::select(int num){
  if(selected != num) selected = num;
  else selected = -1;
  return selected;
}

    
