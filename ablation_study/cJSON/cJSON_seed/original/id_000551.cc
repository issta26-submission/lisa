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
//<ID> 551
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = " { \"a\": 1.5 , \"flag\" : false } ";
    cJSON_Minify(json);
    cJSON *parsed = cJSON_Parse(json);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    cJSON *computed = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON_AddTrueToObject(root, "ready");
    double a_val = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "a"));
    double set_result = cJSON_SetNumberHelper(computed, a_val * 2.0);
    (void)set_result;

    // step 3: Operate
    char *snapshot = cJSON_Print(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double final_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "computed"));
    (void)final_val;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}