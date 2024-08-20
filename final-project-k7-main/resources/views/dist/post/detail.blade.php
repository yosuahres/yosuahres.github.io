@extends('dist.master')
@section('title')
Question Detail
@endsection

@section('content')
<a href="/post" class="btn btn-secondary my-2">Kembali</a>
<img src="{{asset('image/'. $post->image)}}" class="img-fluid" width="40%">
<p>{{$post -> content}}</p>

@endsection