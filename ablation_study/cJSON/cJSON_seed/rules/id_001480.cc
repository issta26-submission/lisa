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
//<ID> 1480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *null_item = NULL;
    cJSON *minified_string_item = NULL;
    cJSON *found_true = NULL;
    char sample_json[] = " { \"pre\" : 123 , \"flag\" : true } ";
    char *printed = NULL;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    cJSON_Minify(sample_json);
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "is_valid", true_item);
    cJSON_AddItemToObject(root, "opt", null_item);
    minified_string_item = cJSON_CreateString(sample_json);
    cJSON_AddItemToObject(root, "sample", minified_string_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    found_true = cJSON_GetObjectItem(root, "is_valid");
    is_true = cJSON_IsTrue(found_true);
    (void)is_true;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}