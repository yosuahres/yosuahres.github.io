<!DOCTYPE html>
<html lang="en">

<head>
    <title>@yield('title') </title>
    <!-- HTML5 Shim and Respond.js IE11 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 11]>
    	<script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
    	<script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
    	<![endif]-->
    <!-- Meta -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=0, minimal-ui">
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="description" content="" />
    <meta name="keywords" content="">
    <meta name="author" content="Phoenixcoded" />
    <!-- Favicon icon -->
    <link rel="icon" href="{{asset('./template/images/favicon.ico')}}" type="image/x-icon">

    <!-- vendor css -->
    <link rel="stylesheet" href="{{asset('./template/css/style.css')}}">
    
    

</head>
<body class="">
	<!-- [ Pre-loader ] start -->
	{{-- <div class="loader-bg">
		<div class="loader-track">
			<div class="loader-fill"></div>
		</div>
	</div> --}}
	<!-- [ Pre-loader ] End -->

	<!-- [ navigation menu ] start -->
	@include('dist.partials.navigation')
	<!-- [ navigation menu ] end -->

	<!-- [ Header ] start -->
	@include('dist.partials.header')
	<!-- [ Header ] end -->
	
	

<!-- [ Main Content ] start -->
<div class="pcoded-main-container">
    <div class="pcoded-content">
        <!-- [ breadcrumb ] start -->
        <div class="page-header">
            <div class="page-block">
                <div class="row align-items-center">
                    <div class="col-md-12">
                        <div class="page-header-title">
                            <h5 class="m-b-10">@yield('title')</h5>
                        </div>
                        {{-- <ul class="breadcrumb">
                            <li class="breadcrumb-item"><a href="/dist/master"><i class="feather icon-home"></i></a></li>
                            <li class="breadcrumb-item"><a href="#!">Question Forum</a></li>
                        </ul> --}}
                    </div>
                </div>
            </div>
        </div>

    {{-- @include('dist.partials.content') --}}
    @yield('content')
        <!-- [ Main Content ] end -->
    </div>
</div>
<!-- [ Main Content ] end -->

    <!-- Required Js -->
    <script src="{{asset('./template/js/vendor-all.min.js')}}"></script>
    <script src="{{asset('./template/js/plugins/bootstrap.min.js')}}"></script>
    <script src="{{asset('./template/js/pcoded.min.js')}}"></script>

<!-- Apex Chart -->
<script src="{{asset('./template/js/plugins/apexcharts.min.js')}}"></script>


<!-- custom-chart js -->
<script src="{{asset('./template/js/pages/dashboard-main.js')}}"></script>
</body>

</html>
