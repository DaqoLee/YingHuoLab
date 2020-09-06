// pages/setting/setting.js
var util = require('../../utils/util.js');
Page({

  /**
   * 页面的初始数据
   */
  data: {
    Time:"1",
    dataList:[
      {mid:"1",following:"1",whisper:"1",black:"1",follower:"1"}
    ]
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    let that = this

    // wx.request({
    //   url: 'https://api.bilibili.com/x/relation/stat?vmid=435162639&jsonp=jsonp',
    //   success:res=>{
    //    console.log(res.data);
    //    this.setData({
    //      dataList: res.data
    //    })
    //   }
    // })

    var time = util.formatTime(new Date());
    // 再通过setData更改Page()里面的data，动态更新页面的数据
    this.setData({
      Time: time
    });

    wx.setNavigationBarTitle({
      title: '设置',
    })
    wx.getStorage({
      key: 'filterNoName',
      success (res) { 
        let filterNoName = res.data
        that.setData({
          filterNoName:filterNoName
        })
      },fail(res){
        console.log(res)
        wx.setStorage({
          key:"filterNoName",
          data:false
        })
        that.setData({
          filterNoName:false
        })
      }
    })
  },
    /**
   * 
   * @param {*} event 
   */
  filterDevice(event) {
    this.setData({
      filterNoName: event.detail.value
    })
    wx.setStorage({
      key:"filterNoName",
      data:event.detail.value
    })

  },


})