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
//<ID> 869
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    float nums[3] = { 1.1f, 2.2f, 3.3f };
    cJSON *farr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(child, "floats", farr);
    cJSON *flag_false = cJSON_AddFalseToObject(child, "active");
    (void)flag_false;

    // step 2: Configure
    char json_buf[] = " { \"external\" : { \"x\": 7, \"y\": 8 } } ";
    cJSON_Minify(json_buf);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_buf, &parse_end, 1);
    cJSON *external = cJSON_GetObjectItem(parsed, "external");
    cJSON *external_ref = cJSON_CreateObjectReference(external);
    cJSON_AddItemToObject(root, "external", external_ref);

    // step 3: Operate
    char outbuf[256];
    memset(outbuf, 0, sizeof(outbuf));
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, outbuf, (int)sizeof(outbuf), 0);
    (void)pre_ok;
    char *raw = cJSON_PrintUnformatted(root);
    int float_count = cJSON_GetArraySize(farr);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(farr, 0));
    (void)float_count;
    (void)first_val;

    // step 4: Validate & Cleanup
    cJSON_free(raw);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}