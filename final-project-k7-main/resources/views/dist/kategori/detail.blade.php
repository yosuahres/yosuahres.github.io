@extends('dist.master')
@section('title')
Category Detail
@endsection

@section('content')
<h1>{{$kategori->name}}</h1>

<a href="/cast" class="btn btn sm my-3"></a>

@endsection