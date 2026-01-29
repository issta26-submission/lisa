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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"alpha\":[1,2,3],\"beta\":{\"inner\":\"val\"}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *extra = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(extra, n1);
    cJSON_AddItemToArray(extra, n2);
    cJSON_AddItemToObject(root, "extra", extra);
    const char *ver = cJSON_Version();
    cJSON *verstr = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "lib_version", verstr);

    // step 3: Operate and Validate
    cJSON_bool hasAlpha = cJSON_HasObjectItem(root, "alpha");
    cJSON *alpha = cJSON_GetObjectItem(root, "alpha");
    int alpha_size = cJSON_GetArraySize(alpha);
    double computed_val = (double)alpha_size + (double)hasAlpha * 10.0;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}