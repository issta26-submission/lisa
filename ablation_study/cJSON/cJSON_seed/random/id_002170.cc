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
//<ID> 2170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[48];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *t_item = NULL;
    cJSON *f_item = NULL;
    cJSON *added_direct = NULL;
    cJSON *got_true = NULL;
    char *printed = NULL;
    cJSON_bool is_root_array = 0;
    cJSON_bool is_true_array = 0;
    int validation = 0;

    // step 2: Setup (create object and add boolean entries in different ways)
    root = cJSON_CreateObject();
    added_direct = cJSON_AddBoolToObject(root, "direct_bool", 1);
    t_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "created_true", t_item);
    f_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "created_false", f_item);

    // step 3: Operate (serialize and probe items)
    printed = cJSON_PrintUnformatted(root);
    got_true = cJSON_GetObjectItem(root, "created_true");
    is_root_array = cJSON_IsArray(root);
    is_true_array = cJSON_IsArray(got_true);

    // step 4: Validate (accumulate simple checks into validation)
    validation ^= (int)(root != NULL);
    validation ^= (int)(added_direct != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(is_root_array == 0);
    validation ^= (int)(is_true_array == 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}