#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 903
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *active_flag = cJSON_AddTrueToObject(meta, "active");
    double measurements[4] = {1.0, 2.5, 3.75, 4.125};
    cJSON *measure_arr = cJSON_CreateDoubleArray(measurements, 4);
    cJSON_AddItemToObject(meta, "measurements", measure_arr);
    cJSON_AddStringToObject(root, "title", "Original Title");
    const char *version_literal = "v1.2.3";
    cJSON *version_ref = cJSON_CreateStringReference(version_literal);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON *replacement_title_ref = cJSON_CreateStringReference("Updated Title");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "title", replacement_title_ref);
    (void)replaced;

    // step 3: Operate
    int count = cJSON_GetArraySize(measure_arr);
    cJSON *m0 = cJSON_GetArrayItem(measure_arr, 0);
    cJSON *m1 = cJSON_GetArrayItem(measure_arr, 1);
    cJSON *m2 = cJSON_GetArrayItem(measure_arr, 2);
    cJSON *m3 = cJSON_GetArrayItem(measure_arr, 3);
    double v0 = cJSON_GetNumberValue(m0);
    double v1 = cJSON_GetNumberValue(m1);
    double v2 = cJSON_GetNumberValue(m2);
    double v3 = cJSON_GetNumberValue(m3);
    double total = v0 + v1 + v2 + v3;
    double average = total / (double)count;
    cJSON_AddNumberToObject(meta, "average", average);
    void *buf = cJSON_malloc(512);
    memset(buf, 0, 512);
    char *prebuf = (char *)buf;
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 512, 1);
    (void)pre_ok;
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_bool is_active = cJSON_IsTrue(active_flag);
    const char *ver_str = cJSON_GetStringValue(version_ref);
    double validate_avg = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "average"));
    (void)is_active;
    (void)ver_str;
    (void)validate_avg;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}