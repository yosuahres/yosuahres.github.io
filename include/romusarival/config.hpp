#ifndef ROMUSARIVAL_CONFIG_HPP
#define ROMUSARIVAL_CONFIG_HPP
#include <ros/package.h>
#include <ros/ros.h>
#include <yaml-cpp/yaml.h>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class ConfigParser {
 public:
  ConfigParser(const std::string &filename);

  const YAML::Node &get() const { return config; }

  void reload(const std::string &filename);

  template <typename T>
  T get_config_value(std::string parent_key, std::string child_key) const {
    try {
      if (!config[parent_key][child_key]) {
        ROS_ERROR("Key %s[%s] not found in config file", parent_key.c_str(), child_key.c_str());
        return T();
      }
      return config[parent_key][child_key].as<T>();
    } catch (const YAML::BadConversion &e) {
      ROS_ERROR("Failed to convert value of key %s[%s] to the requested type: %s", parent_key.c_str(), child_key.c_str(), e.what());
      return T();
    } catch (const YAML::Exception &e) {
      ROS_ERROR("Error occurred while accessing key %s[%s]: %s", parent_key.c_str(), child_key.c_str(), e.what());
      return T();
    }
  }

  float get_target_x(std::string parent_key) const { return get_config_value<float>(parent_key, "x"); }
  float get_target_y(std::string parent_key) const { return get_config_value<float>(parent_key, "y"); }
  float get_target_sudut(std::string parent_key) const { return get_config_value<float>(parent_key, "sudut"); }
  float get_vel_x(std::string parent_key) const { return get_config_value<float>(parent_key, "vel_x"); }
  float get_vel_y(std::string parent_key) const { return get_config_value<float>(parent_key, "vel_y"); }
  float get_vel_w(std::string parent_key) const { return get_config_value<float>(parent_key, "vel_w"); }
  float get_target_cam(std::string parent_key) const { return get_config_value<float>(parent_key, "pix"); }

 private:
  YAML::Node config;
};
#endif