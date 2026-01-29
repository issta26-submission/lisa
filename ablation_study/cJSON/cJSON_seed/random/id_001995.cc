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
//<ID> 1995
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
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *label = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_label = NULL;
    char *json_str = NULL;
    cJSON_bool added_array = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_label = 0;
    int validation = 0;

    // step 2: Setup (Initialize root and container)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    added_array = cJSON_AddItemToObject(root, "items", arr);

    // step 3: Core operations (create strings and add to array and object)
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    label = cJSON_CreateString("alpha_label");
    added1 = cJSON_AddItemToArray(arr, s1);
    added2 = cJSON_AddItemToArray(arr, s2);
    added_label = cJSON_AddItemToObject(root, "label", label);

    // step 4: Query and serialize
    got_arr = cJSON_GetObjectItem(root, "items");
    got_label = cJSON_GetObjectItem(root, "label");
    json_str = cJSON_PrintUnformatted(root);

    // step 5: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(s1 != NULL);
    validation ^= (int)(s2 != NULL);
    validation ^= (int)(label != NULL);
    validation ^= (int)(added_array != 0);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(added_label != 0);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(got_label != 0);
    validation ^= (int)(json_str != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}