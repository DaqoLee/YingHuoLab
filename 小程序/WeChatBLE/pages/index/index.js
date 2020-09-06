//index.js
//获取应用实例
const app = getApp()
Page({
  data: {
    errMsg: '',
    filterNoName:false,
    devices: [],
  },
  setting(res){
    wx.navigateTo({
      url: '/pages/setting/setting',
    })
  },

  onShow: function () {
    let that = this
    wx.setNavigationBarTitle({
      title: '萤火',
    })

    wx.getStorage({
      key: 'filterNoName',
      success (res) { 
        let filterNoName = res.data
        that.setData({
          filterNoName:filterNoName
        })
      },fail(res){ 
        wx.setStorage({
          key:"filterNoName",
          data:false
        })
        that.setData({
          filterNoName:false
        })
      }
    })

    that.setData({
      devices: []
    })
    wx.openBluetoothAdapter({
      success(res) {

        wx.getBluetoothAdapterState({
          success(res) {
            // console.log(res)

            let available = res.available
            if (available) {
              let discovering = res.discovering
              if (discovering) {
                that.onBluetoothDeviceFound()
              } else {
                wx.startBluetoothDevicesDiscovery({
                  allowDuplicatesKey: true,
                  success(res) {
                    that.onBluetoothDeviceFound()
                  },
                  fail(res) {
                    that.setData({
                      errMsg: res.errMsg
                    })
                  }
                })
              }
            } else {
              that.setData({
                errMsg: res.errMsg
              })
            }

          }
        })
      },
      fail(res) {
        //console.log('fail',res)
        that.setData({
          errMsg: res.errMsg
        })
      }
    })
    console.log('onShow')
  },
  /**
   * 连接蓝牙
   * @param {*} e 
   */
  createBLEConnection(e) {
    // console.log(e)
    let that = this
    const ds = e.currentTarget.dataset
    const deviceId = ds.deviceId
    const name = ds.name

    wx.showActionSheet({
      itemList: ['快速/智能连接', '普通连接'],
      success(res) {
        // console.log('showActionSheet success', res)

        let index = res.tapIndex
        if (index == 0) {
          // 快速连接/智能连接
          that.smartConnect(deviceId,name)
        } else {
          // 选择service ///pages/choose_service/choose_service?deviceId=
          wx.navigateTo({
            url: '/pages/choose_service/choose_service?deviceId=' + deviceId + '&name=' + name,
          })
        }
      },
      fail(res) {
        // console.log('showActionSheet fail', res)
      }
    })
  },
  /**
   * 快速/智能连接
   * @param {*} deviceId 
   */
  smartConnect(deviceId,name) {
    let that = this
    wx.showLoading({
      title: 'connecting...',
    })
    wx.createBLEConnection({
      deviceId,
      success: (res) => {
        // console.log(res)
        wx.getBLEDeviceServices({
          deviceId: deviceId,
          success(res) {
            // console.log(res)
            let services = res.services
            for (let i = 0; i < services.length; i++) {
              let uuid = services[i].uuid
              let serviceUUID = services[i]
              // console.log(uuid)
              // console.log(uuid.indexOf('0000FFE0'))
              // console.log(uuid.indexOf('0000FFE0') > 0)
              if (uuid.indexOf('0000FFE0') >= 0) {
                let serviceId = uuid
                wx.getBLEDeviceCharacteristics({
                  deviceId: deviceId,
                  serviceId: serviceId,
                  success(res) {
                    console.log(res)

                    let characteristics = res.characteristics
                    for (let i = 0; i < characteristics.length; i++) {
                      let uuid = characteristics[i].uuid
                      if (uuid.indexOf('0000FFE1') >= 0) {
                        let obj = {
                          characteristicsUUID: characteristics[i],
                          deviceId: deviceId,
                          serviceUUID: serviceUUID,
                          deviceName:name,
                          needClose:true
                        }

                        app.globalData.isConnect = true
                        var data = JSON.stringify(obj);

                        wx.hideLoading({
                          complete: (res) => {

                            wx.navigateTo({
                              url: '/pages/psy_send/psy_send?data=' + data,
                            })
    
                          },
                        })
                     
                        return;
                      }
                    }

                    that.connectFail(deviceId)
                  },
                  fail(res) {
                    that.connectFail(deviceId)
                  }
                })
                return;
              }
            }
            that.connectFail(deviceId)
          },
          fail(res) {
            that.connectFail(deviceId)
          }
        })
      },
      fail: (res) => {
        that.connectFail(deviceId)
      }
    })
  },
  connectFail(deviceId) {
    wx.hideLoading({
      complete: (res) => {
        app.globalData.isConnect = true
        wx.showToast({
          title: '快速/智能连接失败',
          icon: 'none',
          duration: 1000
        })

        wx.closeBLEConnection({
          deviceId: deviceId,
        })
      },
    })
  },

  onHide: function () {
    wx.offBluetoothDeviceFound()
    wx.stopBluetoothDevicesDiscovery()
  },
  onUnload: function () {
    wx.closeBluetoothAdapter()
    console.log('onUnload')
  },
  /**
   * 测试跳转
   * @param {*} res 
   */
  test(res){
    wx.navigateTo({
      url: '/pages/connet/connet',
    })
  },
  onBluetoothDeviceFound() {
    let that = this
    wx.onBluetoothDeviceFound(function (res) {

      res.devices.forEach(device => {
        if (!device.name && !device.localName) {
          if(that.data.filterNoName){
            return
          }
          device.name = '未知设备'
          // return
        }
        const foundDevices = that.data.devices
        const idx = that.inArray(foundDevices, 'deviceId', device.deviceId)
        const data = {}
        if (idx === -1) {
          data[`devices[${foundDevices.length}]`] = device
        } else {
          data[`devices[${idx}]`] = device
        }
        that.setData(data)
      })
    })
  },
  // ArrayBuffer转16进度字符串示例
  ab2hex(buffer) {
    var hexArr = Array.prototype.map.call(
      new Uint8Array(buffer),
      function (bit) {
        return ('00' + bit.toString(16)).slice(-2)
      }
    )
    return hexArr.join('');
  },
  inArray(arr, key, val) {
    for (let i = 0; i < arr.length; i++) {
      if (arr[i][key] === val) {
        return i;
      }
    }
    return -1;
  }
})