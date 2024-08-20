<?php

use App\Http\Controllers\ProfileController;
use Illuminate\Support\Facades\Route;
use App\Http\Controllers\PostController;
use App\Http\Controllers\KategoriController;

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', function () {
    return view('dist.home');
});

Route::group(['middleware' => ['auth']], function () {
    //CRUD category
    // Create Data
    // ke form tambah
    Route::get('/kategori/create', [KategoriController::class, 'create']);

    //Route tambah data ke database
    Route::post('/kategori', [KategoriController::class, 'store']);

    //Read Data
    //untuk tampil semua cast
    Route::get('/kategori', [KategoriController::class, 'index']);
    //untuk detail cast berdasarkan id
    Route::get('/kategori/{id}', [KategoriController::class, 'show']);

    //updateData
    //route untuk mengarah ke form edit data dengan params id;
    Route::get('/kategori/{id}/edit', [KategoriController::class, 'edit']);
    //route update data di database
    Route::put('/kategori/{id}', [KategoriController::class, 'update']);

    //delete data;
    Route::delete('kategori/{id}', [KategoriController::class, 'destroy']);

    //update Profile
    Route::get('/profile', [ProfileController::class, 'index']);

});




//CRUD Post
Route::resource('post', PostController::class);

Auth::routes();