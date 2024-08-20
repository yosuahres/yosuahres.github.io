@extends('dist.master')
@section('title')
Welcome to My Website
@endsection
@section('content')
<nav class="navbar navbar-expand-lg navbar-light bg-light">
    <a class="navbar-brand" href="/">My Website</a>
    <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
    </button>
    <div class="collapse navbar-collapse" id="navbarNav">
        <ul class="navbar-nav">
            <li class="nav-item active">
                <a class="nav-link" href="/">Home <span class="sr-only">(current)</span></a>
            </li>
            <li class="nav-item">
                <a class="nav-link" href="/post/create">Create a New Post</a>
            </li>
            <li class="nav-item">
                <a class="nav-link" href="/post">See Other People's Posts</a>
            </li>
        </ul>
    </div>
</nav>

<div class="container mt-5">
    <div class="jumbotron text-center">
        <h1 class="display-4">Welcome to My Website</h1>
        <p class="lead">Discover and share amazing posts from around the world.</p>
    </div>
</div>

<footer class="bg-dark text-white text-center py-3">
    &copy; 2023 Ricardo Supriyanto and Yosua Hares
</footer>
@endsection
