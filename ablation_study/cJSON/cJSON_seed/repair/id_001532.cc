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
//<ID> 1532
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
    cJSON *active_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", active_bool);

    // step 2: Configure
    cJSON *pi_num = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(meta, "pi", pi_num);
    double pi_val = cJSON_GetNumberValue(pi_num);
    cJSON *two_pi_num = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(root, "two_pi", two_pi_num);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = name_item && name_item->valuestring && name_item->valuestring[0] ? name_item->valuestring[0] : 'x';
    scratch[1] = (char)('0' + ((int)pi_val % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('#');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}