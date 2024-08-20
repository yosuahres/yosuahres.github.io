<nav class="pcoded-navbar  ">
    <div class="navbar-wrapper  ">
        <div class="navbar-content scroll-div ">

            <div class="">
                <div class="main-menu-header">
                    <img class="img-radius" src="{{ asset('./template/images/user/avatar-2.jpg') }}"
                        alt="User-Profile-Image">
                    @guest
                        <div class="user-details">
                            <span>Login</span>
                        </div>
                    @endguest
                    @auth
                        <div class="user-details">
                            <span>{{ Auth::user()->name }}  ({{ Auth::user()->profile->age }} Tahun)</span>
                            <div id="more-details">{{ Auth::user()->profile->address }}<i class="fa fa-chevron-down m-l-5"></i></div>
                        </div>
                    @endauth
                </div>
                <div class="collapse" id="nav-user-link">
                    <ul class="list-unstyled">
                        @auth
                            <li class="list-group-item"><a href="/profile"><i class="feather icon-user m-r-5"></i>View
                                    Profile</a></li>
                            <li class="list-group-item"><a href="#!"><i
                                        class="feather icon-settings m-r-5"></i>Settings</a></li>
                            <li class="list-group-item"><a href="{{ route('logout') }}"
                                    onclick="event.preventDefault();
                                   document.getElementById('logout-form').submit();">
                                    LogOut
                                </a>

                                <form id="logout-form" action="{{ route('logout') }}" method="POST" class="d-none">
                                    @csrf
                                </form>
                            </li>
                        @endauth
                    </ul>
                </div>
            </div>

            <ul class="nav pcoded-inner-navbar ">
                <li class="nav-item pcoded-menu-caption">
                    <label>Navigation</label>
                </li>
                <li class="nav-item">
                    <a href="/post" class="nav-link "><span class="pcoded-micon"><i
                                class="feather icon-home"></i></span><span class="pcoded-mtext">Question Post</span></a>
                </li>

                @guest
                    <li class="nav-item">
                        <a href="/login" class="nav-link "><span class="pcoded-mtext">Login</span></a>
                    </li>
                @endguest
            </ul>
</nav>
