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
//<ID> 1778
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
    cJSON_AddStringToObject(meta, "name", "device-01");
    cJSON_AddNumberToObject(meta, "version", 3.14);
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddNumberToObject(metrics, "cpu", 42.5);
    cJSON_AddStringToObject(metrics, "status", "ok");
    cJSON *readings = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "readings", readings);
    cJSON *r0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(readings, r0);
    cJSON *r1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(readings, r1);
    cJSON *r2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(readings, r2);

    // step 2: Configure
    cJSON_InsertItemInArray(readings, 1, cJSON_CreateNumber(1.5));
    double updated_cpu = cJSON_SetNumberHelper(cJSON_GetObjectItem(metrics, "cpu"), 55.0);
    cJSON *snapshot = cJSON_Duplicate(root, 1);

    // step 3: Operate and Validate
    cJSON *meta_snap = cJSON_GetObjectItem(snapshot, "meta");
    cJSON *name_item = cJSON_GetObjectItem(meta_snap, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON *cpu_item = cJSON_GetObjectItem(cJSON_GetObjectItem(snapshot, "metrics"), "cpu");
    double cpu_value = cJSON_GetNumberValue(cpu_item);
    char *outbuf = (char *)cJSON_malloc(64);
    memset(outbuf, 0, 64);
    char *printed = cJSON_PrintUnformatted(snapshot);
    outbuf[0] = printed ? printed[0] : 'X';
    outbuf[1] = name_is_string ? (name_value ? name_value[0] : 'Z') : 'N';
    int count_readings = cJSON_GetArraySize(cJSON_GetObjectItem(cJSON_GetObjectItem(snapshot, "metrics"), "readings"));
    outbuf[2] = (char)('0' + (count_readings % 10));
    (void)cpu_value;
    (void)updated_cpu;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(outbuf);
    cJSON_Delete(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}