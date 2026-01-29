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
//<ID> 2002
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
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *got_str = NULL;
    cJSON *bool_item = NULL;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup (create root, array, and a numeric item)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(arr, num_item);

    // step 3: Configure (attach array and other members to root, add a bool via required API)
    cJSON_AddItemToObject(root, "values", arr);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    str_item = cJSON_CreateString("ok");
    cJSON_AddItemToObject(root, "status", str_item);

    // step 4: Operate (inspect array size and retrieve string value using required APIs)
    arr_size = cJSON_GetArraySize(arr);
    got_str = cJSON_GetObjectItem(root, "status");
    char *sval = cJSON_GetStringValue(got_str);
    buf[0] = sval[0];
    buf[1] = (char)('0' + (arr_size % 10));

    // step 5: Validate (aggregate presence checks into validation; no branches)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(bool_item != NULL);
    buf[2] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}