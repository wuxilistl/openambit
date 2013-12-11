/*
 * (C) Copyright 2013 Emil Ljungdahl
 *
 * This file is part of Openambit.
 *
 * Openambit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contributors:
 *
 */
#include "logentry.h"

LogEntry::LogEntry()
{
}

LogEntry::LogEntry(const LogEntry &other)
{
    u_int32_t i;

    if (other.personalSettings != NULL) {
        personalSettings = (ambit_personal_settings_t*)malloc(sizeof(ambit_personal_settings_t));
        memcpy(personalSettings, other.personalSettings, sizeof(ambit_personal_settings_t));
    }
    else {
        personalSettings = NULL;
    }

    if (other.logEntry != NULL) {
        logEntry = (ambit_log_entry_t*)malloc(sizeof(ambit_log_entry_t));
        memcpy(logEntry, other.logEntry, sizeof(ambit_log_entry_t));
        if (other.logEntry->samples != NULL) {
            logEntry->samples = (ambit_log_sample_t*)malloc(sizeof(ambit_log_sample_t)*other.logEntry->samples_count);
            memcpy(logEntry->samples, other.logEntry->samples, sizeof(ambit_log_sample_t)*other.logEntry->samples_count);
            for (i=0; i<other.logEntry->samples_count; i++) {
                if (other.logEntry->samples[i].type == ambit_log_sample_type_periodic) {
                    if (other.logEntry->samples[i].u.periodic.values != NULL) {
                        logEntry->samples[i].u.periodic.values = (ambit_log_sample_periodic_value_t*)malloc(sizeof(ambit_log_sample_periodic_value_t)*other.logEntry->samples[i].u.periodic.value_count);
                        memcpy(logEntry->samples[i].u.periodic.values, other.logEntry->samples[i].u.periodic.values, sizeof(ambit_log_sample_periodic_value_t)*other.logEntry->samples[i].u.periodic.value_count);
                    }
                }
                if (other.logEntry->samples[i].type == ambit_log_sample_type_gps_base) {
                    if (other.logEntry->samples[i].u.gps_base.satellites != NULL) {
                        logEntry->samples[i].u.gps_base.satellites = (ambit_log_gps_satellite_t*)malloc(sizeof(ambit_log_gps_satellite_t)*logEntry->samples[i].u.gps_base.satellites_count);
                        memcpy(logEntry->samples[i].u.gps_base.satellites, other.logEntry->samples[i].u.gps_base.satellites, sizeof(ambit_log_gps_satellite_t)*logEntry->samples[i].u.gps_base.satellites_count);
                    }
                }
            }
        }
    }
}

LogEntry& LogEntry::operator=(const LogEntry &rhs)
{
    LogEntry tmp(rhs);

    std::swap(device, tmp.device);
    std::swap(time, tmp.time);
    std::swap(personalSettings, tmp.personalSettings);
    std::swap(logEntry, tmp.logEntry);

    return *this;
}

LogEntry::~LogEntry()
{
    u_int32_t i;

    if (personalSettings != NULL) {
        free(personalSettings);
        personalSettings = NULL;
    }

    if (logEntry != NULL) {
        if (logEntry->samples != NULL) {
            for (i=0; i<logEntry->samples_count; i++) {
                if (logEntry->samples[i].type == ambit_log_sample_type_periodic) {
                    if (logEntry->samples[i].u.periodic.values != NULL) {
                        free(logEntry->samples[i].u.periodic.values);
                    }
                }
                if (logEntry->samples[i].type == ambit_log_sample_type_gps_base) {
                    if (logEntry->samples[i].u.gps_base.satellites != NULL) {
                        free(logEntry->samples[i].u.gps_base.satellites);
                    }
                }
            }
            free(logEntry->samples);
        }
        free(logEntry);
    }

    logEntry = NULL;
}
