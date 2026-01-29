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
//<ID> 1983
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
    cJSON *orig_str = NULL;
    cJSON *dup_str = NULL;
    cJSON *flag_item = NULL;
    cJSON *repl_bool = NULL;
    cJSON *flag_ptr = NULL;
    char *out_json = NULL; /* fixed type: cJSON_PrintUnformatted returns char * */
    cJSON_bool has_text = 0;
    cJSON_bool added_orig = 0;
    cJSON_bool replaced = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    orig_str = cJSON_CreateString("hello world");
    added_orig = cJSON_AddItemToObject(root, "text", orig_str);
    flag_item = cJSON_AddBoolToObject(root, "flag", 0);

    // step 3: Core operations
    has_text = cJSON_HasObjectItem(root, "text");
    dup_str = cJSON_Duplicate(orig_str, 1);
    cJSON_AddItemToObject(root, "text_copy", dup_str);
    flag_ptr = cJSON_GetObjectItem(root, "flag");
    repl_bool = cJSON_CreateTrue();
    replaced = cJSON_ReplaceItemViaPointer(root, flag_ptr, repl_bool);
    out_json = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(orig_str != NULL);
    validation ^= (int)(dup_str != NULL);
    validation ^= (int)(flag_item != NULL);
    validation ^= (int)(has_text != 0);
    validation ^= (int)(added_orig != 0);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(out_json != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out_json);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}