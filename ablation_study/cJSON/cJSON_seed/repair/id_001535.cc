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
//<ID> 1535
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
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "fuzz_tester");
    cJSON *ver = cJSON_CreateNumber(1.23);
    cJSON_AddItemToObject(meta, "version", ver);

    // step 2: Configure
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *status = cJSON_AddStringToObject(root, "status", "testing");
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "num1", num1);

    // step 3: Operate and Validate
    double v_num1 = cJSON_GetNumberValue(num1);
    double v_ver = cJSON_GetNumberValue(ver);
    cJSON *sum = cJSON_CreateNumber(v_num1 + v_ver);
    cJSON_AddItemToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    scratch[0] = (char)('0' + ((int)v_num1 % 10));
    scratch[1] = out ? out[0] : ' ';
    scratch[2] = name_item && name_item->valuestring ? name_item->valuestring[0] : 'x';
    scratch[3] = (char)('A');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}