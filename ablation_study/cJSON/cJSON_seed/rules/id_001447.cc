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
//<ID> 1447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *orig = NULL;
    cJSON *replacement = NULL;
    cJSON *raw_item = NULL;
    cJSON *nil_item = NULL;
    cJSON *fetched_orig = NULL;
    cJSON *fetched_raw = NULL;
    char *printed = NULL;
    cJSON_bool replace_ok = 0;
    cJSON_bool isnull_check = 0;
    const char orig_json[] = "{\"alpha\":1}";
    const char repl_json[] = "{\"beta\":\"two\"}";
    const char raw_json[] = "789";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig = cJSON_Parse(orig_json);
    cJSON_AddItemToObjectCS(root, "orig", orig);
    replacement = cJSON_Parse(repl_json);
    replace_ok = cJSON_ReplaceItemViaPointer(root, orig, replacement);
    raw_item = cJSON_AddRawToObject(root, "raw", raw_json);
    nil_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "nil", nil_item);
    isnull_check = cJSON_IsNull(nil_item);

    // step 3: Operate / Validate
    fetched_orig = cJSON_GetObjectItem(root, "orig");
    fetched_raw = cJSON_GetObjectItem(root, "raw");
    printed = cJSON_Print(root);
    (void)replace_ok;
    (void)isnull_check;
    (void)fetched_orig;
    (void)fetched_raw;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}