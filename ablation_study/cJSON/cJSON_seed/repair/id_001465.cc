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
//<ID> 1465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"meta\":{\"version\":1.2},\"items\":[{\"id\":1,\"name\":\"one\"},{\"id\":2,\"name\":\"two\"}],\"active\":true}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");

    // step 2: Configure
    int count = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *first_dup = cJSON_Duplicate(first_item, 1);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "count", (double)count);
    cJSON_AddItemToObject(summary, "sample", first_dup);
    cJSON_AddNumberToObject(summary, "version", cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "version")));

    // step 3: Operate and Validate
    char *out_root = cJSON_PrintUnformatted(root);
    char *out_summary = cJSON_PrintUnformatted(summary);
    size_t len_root = out_root ? (size_t)strlen(out_root) : (size_t)0;
    size_t len_summary = out_summary ? (size_t)strlen(out_summary) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len_root + len_summary + 16);
    memset(scratch, 0, len_root + len_summary + 16);
    scratch[0] = len_root ? out_root[0] : ' ';
    scratch[1] = (char)('0' + (count % 10));
    cJSON *sample = cJSON_GetObjectItem(summary, "sample");
    cJSON *sample_name = cJSON_GetObjectItem(sample, "name");
    char *sname = cJSON_GetStringValue(sample_name);
    scratch[2] = sname ? sname[0] : 'n';
    scratch[3] = len_summary ? out_summary[0] : ' ';
    double ver = cJSON_GetNumberValue(cJSON_GetObjectItem(summary, "version"));
    scratch[4] = (char)('0' + ((int)ver % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out_root);
    cJSON_free(out_summary);
    cJSON_Delete(root);
    cJSON_Delete(summary);
    // API sequence test completed successfully
    return 66;
}