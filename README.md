

<div dir='rtl'>
        <h1>Farri voice Message Application</h1>
        <p> using WebRTC and QT Creator </p>
        <p>این برنامه توسط دو فریم وورک QT Creator و WebRTC نوشته شده است و به وسیله آن می توان 
        .تماس صوتی بین دو دستگاه ایجاد کرد
        </p>
        <br>
        <p>در ادامه توضیحات لازم هر فایل و کارهایی که برای نوشتن این برنامه انجام داده ایم آمده است
        </p>
        <h2> 
        WebRTC
        </h2>
        <p>
        این کتابخانه که توسط گوگل توسعه داده شده هسته اصلی برقراری ارتباط در این برنامه هست.
        این کتابخانه بدون هیچ افزونه و پلاگینی می تواند ارتباط RealTime را برای ما ایجاد کند.
        برای فهمیدن نحوه کار این کتابخانه لازم است ابتدا دو پروتکل اصلی که در این کتابخانه استفاده شده اند یعنی SDP و ICE را بررسی کنیم.
        </p>
        <ul>
                <li>
                        <div>
                                <h3>ICE</h3>
                                <p> یا به طور کامل Interactive Connectivity Establishment protocol پروتکلی است که به وسیله آن کانکشن بین دو دستگاه ایجاد می شود.
                                مزیت این روش این است که موانعی مثل 
                                <a href = 'https://www.digitalsamba.com/blog/ice-and-sdp-in-webrtc#:~:text=NAT%2C%20short%20for%20Network%20Address%20Translator'> NAT </a>
                                 و
                                <a href = 'https://www.digitalsamba.com/blog/ice-and-sdp-in-webrtc#:~:text=NAT%2C%20short%20for%20Network%20Address%20Translator' > firewall </a>
                                 که از ارتباط مستقیم دو دستگاه جلوگیری می کنند را بر می دارد.
                                </p>
                                <br>
                                <p>
                                این روش بدین صورت عمل می کند که طرفین ارتباط با اشتراک گذاری مسیرهایی که می توانند با هم ارتباط برقرار کنند ( که به هر کدام یک candidate گفته می شود) بهترین مسیر برای برقراری ارتباط را تشخیص می دهند و با حذف موانع ذکر شده در بالا با هم ارتباط را برقرار می کنند.
                                </p>
                        </div>
                </li>
                <li>
                        <div>
                                <h3>SDP</h3>
                                <p>Session Description Protocol پروتکلی است که به وسیله آن امکان ارسال فایل های چند رسانه ای در بستر WebRTC امکان پذیر می شود.
                                در واقع این پروتکل باعث ایجاد زبان مشترک بین دو دستگاه می شود و دو دستگاه با به اشتراک گذاری sdp هایشان با هم از نوع فایلی که قرار است ارسال شود ، پروتکلی که تحت آن پیام ها ارسال می شود ، codec و سایر تنظیمات لازم برای برقراری ارتباط آگاه می شوند.
                                </p>
                        </div>
                </li>
        </ul>
        <p>
        پس برای ایجاد ارتباط بین دو نقطه توسط WebRTC لازم است ما اطلاعات SDP و ICE را آنها را با یکدیگر به اشتراک بگذاریم.
        <br>
        برای این کار روش های متفاوتی وجود دارد، برای مثال در 
        <a href = 'https://github.com/paullouisageneau/libdatachannel/tree/master/examples/copy-paste'>یکی از مثال های</a>
        خود کتابخانه این کار به وسیله copy/paste انجام شده است.
        </br>
        </p>
        <p>
        یکی از راه های متداول در انتقال این پیام استفاده Signaling Server است که در این پروژه همین روش استفاده شده است.
        <br>
        Signaling Server یک سرور ساده است که به هر صورتی قابل پیاده سازی است و به وسیله آن تنها SDP و ICE ها منتقل می شوند و بعد از آن دیگر نیازی به آن در برنامه نیست.
        </p>
        <img src='README_files/signal_h.png'>
        <p>
        ما از یک سرور TCP برای اینکار استفاده کرده ایم که لیستی از تمام کسانی که به آن وصل می شوند را نگه می دارد و این امکان را ایجاد می کند که با استفاده از سوکت به آن وصل شوند و با دانستن نام کسی که می خواهند به آن پیام دهند ، پیامشان را از طریق سرور به آن انتقال دهند.
        </p>
        <img src='README_files/signaling_handle.png'>
        <p>
        مهمترین تابع signaling_server تابع handleRequests است که در بالا آمده.
        <br>
        این تابع بر اساس پیام ارسال شده به آنیکی دو کار ریدایرکت کردن پیام به فرد مورد نظر و اضافه کردن فرد به لیست مخاطبین سرور را انجام می دهد
        </p>
        <br>
        <p>
        دو کلاس اصلی در برنامه وجود دارد که آنها answerer و offerer هستند.
        کلاس offerer کلاسی است که شروع کننده مکالمه (تماس گیرنده است ) و answerer کسی است که با آن تماس گرفته می شود.
        توابع مهم و مشترک بین این دو کلاس در زیر آمده است .
        </p>
        <p>
        <img src= 'README_files/initialize_peer.png'>
        initialize_peer_connection :
        این تابع مواردی 
        </p>
        <p>
        <img src='README_files/prepare_sdp.png' >
        prepare_sdp_and_candidate_message:
        این تابع پیامی که شامل SDP و ICE که باید از طریق سیگنالینگ سرور ارسال شود را آماده می کند. فرمت استفاده شده برای پیام json می باشد.
        </p>



</div>
