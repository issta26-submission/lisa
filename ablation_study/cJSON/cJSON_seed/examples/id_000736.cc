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
//<ID> 736
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"meta\":{\"name\":\"tester\"},\"values\":[\"one\",\"two\"]}";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *arr = cJSON_CreateArray();
    cJSON *ref1 = cJSON_CreateStringReference("refA");
    cJSON *ref2 = cJSON_CreateStringReference("refB");

    // step 2: Configure
    cJSON_AddItemToArray(arr, ref1);
    cJSON_AddItemToArray(arr, ref2);
    cJSON_AddItemToObject(parsed, "newrefs", arr);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(parsed);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *reparsed = cJSON_Parse(buffer);
    cJSON *values = cJSON_GetObjectItem(parsed, "values");
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *meta_name = cJSON_GetObjectItem(meta, "name");
    cJSON *newrefs = cJSON_GetObjectItem(parsed, "newrefs");
    int newrefs_count = cJSON_GetArraySize(newrefs);
    cJSON *first_value = cJSON_GetArrayItem(values, 0);
    const char *first_value_str = cJSON_GetStringValue(first_value);
    cJSON *first_newref = cJSON_GetArrayItem(newrefs, 0);
    const char *first_newref_str = cJSON_GetStringValue(first_newref);
    (void)meta_name;
    (void)newrefs_count;
    (void)first_value_str;
    (void)first_newref_str;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(reparsed);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}