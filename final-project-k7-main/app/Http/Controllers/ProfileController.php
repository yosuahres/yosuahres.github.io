<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use App\Models\Profile;

class ProfileController extends Controller
{
    public function index()
    {
        $users_id = Auth::id();

        $profile = profile::where('users_id', $users_id)->first();

        return view('dist.profile.index', ['profile' => $profile]);
    }
}