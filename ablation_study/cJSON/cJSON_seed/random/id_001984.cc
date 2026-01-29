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
//<ID> 1984
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *orig = NULL;
    cJSON *orig_item = NULL;
    cJSON *dup_item = NULL;
    cJSON *flag_item = NULL;
    char *out = NULL;
    cJSON_bool added_orig = 0;
    cJSON_bool has_key = 0;
    cJSON_bool replaced = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("original");
    added_orig = cJSON_AddItemToObject(root, "key", orig);

    // step 3: Core operations
    orig_item = cJSON_GetObjectItem(root, "key");
    has_key = cJSON_HasObjectItem(root, "key");
    dup_item = cJSON_Duplicate(orig_item, 1);
    replaced = cJSON_ReplaceItemViaPointer(root, orig_item, dup_item);
    flag_item = cJSON_AddBoolToObject(root, "flag", 1);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(orig != NULL);
    validation ^= (int)(added_orig != 0);
    validation ^= (int)(has_key != 0);
    validation ^= (int)(dup_item != NULL);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(flag_item != NULL);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}