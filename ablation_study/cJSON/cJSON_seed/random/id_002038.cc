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
//<ID> 2038
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
    cJSON *elem1 = NULL;
    cJSON *elem2 = NULL;
    cJSON *name_item = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *retrieved_name = NULL;
    cJSON_bool added_elem1 = 0;
    cJSON_bool added_elem2 = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_name = 0;
    char *setret = NULL;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem1 = cJSON_CreateString("first");
    elem2 = cJSON_CreateString("second");
    name_item = cJSON_CreateString("original");

    // step 3: Configure
    added_elem1 = cJSON_AddItemToArray(arr, elem1);
    added_elem2 = cJSON_AddItemToArray(arr, elem2);
    added_arr = cJSON_AddItemToObject(root, "list", arr);
    added_name = cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate / Inspect
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_name = cJSON_GetObjectItem(root, "name");
    setret = cJSON_SetValuestring(retrieved_name, "updated");

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(elem1 != NULL);
    validation ^= (int)(elem2 != NULL);
    validation ^= (int)(retrieved_arr != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(retrieved_name != NULL);
    validation ^= (int)(setret != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}