/* include/linux/msm_hdmi.h
 *
 * Copyright (c) 2014 The Linux Foundation. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef _MSM_HDMI_H_
#define _MSM_HDMI_H_

/*
                                       
                                  
                                                 
                                                       
 */
struct hdmi_cable_notify {
	struct list_head link;
	int status;
	void (*hpd_notify) (struct hdmi_cable_notify *h);
};

#ifdef CONFIG_FB_MSM_MDSS_HDMI_PANEL
/*
                                                              
                                                   
                                                                    
 */
int register_hdmi_cable_notification(
		struct hdmi_cable_notify *handler);

/*
                                                                 
                                                   
                                  
 */
int unregister_hdmi_cable_notification(
		struct hdmi_cable_notify *handler);
#else
int register_hdmi_cable_notification(
		struct hdmi_cable_notify *handler) {
	return 0;
}

int unregister_hdmi_cable_notification(
		struct hdmi_cable_notify *handler) {
	return 0;
}
#endif /*                               */

#endif /*            */
