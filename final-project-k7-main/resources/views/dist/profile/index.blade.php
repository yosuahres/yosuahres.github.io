@extends('dist.master')
@section('title')
    Update Profile
@endsection
@section('content')
    <form action="/profile/{{$profile->id}}" method="POST">
        @csrf
        @method('put')
        <div class="form-group">
            <label>Name</label>
            <input type="text" value="{{ $profile->user->name }}" name="name"
                class="form-control" disabled>
        </div>
        <div class="form-group">
            <label>Age</label>
            <input type="number" value="{{ $profile->age }}" name="age"
                class="@error('age') is-invalid @enderror form-control" placeholder="Enter Age">
        </div>
        @error('age')
            <div class="alert alert-danger">{{ $message }}</div>
        @enderror
        <div class="form-group">
            <label>Bio</label>
            <textarea class="@error('bio') is-invalid @enderror form-control" name="bio" cols="30" rows="10"
                placeholder="Enter Bio">{{ $profile->bio }}</textarea>
        </div>
        @error('bio')
            <div class="alert alert-danger">{{ $message }}</div>
        @enderror
        <div class="form-group">
            <label>Email</label>
            <input type="text" value="{{ $profile->user->email }}" name="email"
                class="form-control" disabled>
        </div>
        @error('email')
            <div class="alert alert-danger">{{ $message }}</div>
        @enderror
        <div class="form-group">
            <label>Address</label>
            <input type="text" value="{{ $profile->address }}" name="address"
                class="@error('address') is-invalid @enderror form-control" placeholder="Enter Address">
        </div>
        @error('address')
            <div class="alert alert-danger">{{ $message }}</div>
        @enderror


        <button type="submit" class="btn btn-primary">Submit</button>
    </form>
@endsection
