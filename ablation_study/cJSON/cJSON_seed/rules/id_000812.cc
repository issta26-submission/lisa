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
//<ID> 812
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *raw = NULL;
    cJSON *flag = NULL;
    cJSON *other = NULL;
    cJSON *other_raw = NULL;
    cJSON *got_raw = NULL;
    cJSON *dup_raw = NULL;
    const char *raw_text = NULL;
    char buffer[256];
    cJSON_bool printed_ok = 0;
    cJSON_bool cmp_result = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw = cJSON_CreateRaw("{\"meta\":true,\"count\":5}");
    cJSON_AddItemToObject(root, "meta_raw", raw);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    other = cJSON_CreateObject();
    other_raw = cJSON_CreateRaw("{\"meta\":true,\"count\":5}");
    cJSON_AddItemToObject(other, "meta_raw", other_raw);
    cJSON_AddItemToObject(other, "flag", cJSON_CreateFalse());

    // step 3: Operate / Validate
    got_raw = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    raw_text = cJSON_GetStringValue(got_raw);
    dup_raw = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(other, "meta_raw_dup", dup_raw);
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cmp_result = cJSON_Compare(root, other, 1);

    // step 4: Cleanup
    cJSON_Delete(other);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)printed_ok;
    (void)cmp_result;
    (void)raw_text;
    return 66;
}