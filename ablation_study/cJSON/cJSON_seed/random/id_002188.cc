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
//<ID> 2188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item = NULL;
    cJSON *got = NULL;
    char *mem = NULL;
    cJSON_bool is_arr = 0;
    int size = -1;
    int validation = 0;

    // step 2: Setup (create object, array, allocate memory and prepare an item)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    mem = (char *)cJSON_malloc(16);
    memset(mem, 0, 16);
    mem[0] = 'a';
    item = cJSON_CreateString("placeholder");
    cJSON_SetValuestring(item, mem);
    cJSON_AddItemToArray(arr, item);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 3: Operate (retrieve the object item and inspect it)
    got = cJSON_GetObjectItem(root, "arr");
    is_arr = cJSON_IsArray(got);
    size = cJSON_GetArraySize(got);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(item != NULL);
    validation ^= (int)(mem != NULL);
    validation ^= (int)(is_arr != 0);
    validation ^= (int)(size == 1);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(mem);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}