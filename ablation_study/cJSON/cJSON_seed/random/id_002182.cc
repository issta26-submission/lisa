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
//<ID> 2182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[128];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num = NULL;
    cJSON *stritem = NULL;
    cJSON *got_arr = NULL;
    char *printed = NULL;
    cJSON_bool added_num = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_obj = 0;
    cJSON_bool isarr = 0;
    int arrsize = 0;
    int validation = 0;
    void *mem = NULL;

    // step 2: Setup (initialize objects and allocate memory)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(42.0);
    mem = cJSON_malloc(16);
    memset(mem, 0, 16);
    ((char *)mem)[0] = 't';
    ((char *)mem)[1] = 'e';
    ((char *)mem)[2] = 's';
    ((char *)mem)[3] = 't';
    ((char *)mem)[4] = 0;
    stritem = cJSON_CreateString((const char *)mem);
    added_num = cJSON_AddItemToArray(arr, num);
    added_str = cJSON_AddItemToArray(arr, stritem);
    added_obj = cJSON_AddItemToObject(root, "data", arr);

    // step 3: Operate (serialize and retrieve)
    printed = cJSON_PrintUnformatted(root);
    got_arr = cJSON_GetObjectItem(root, "data");
    isarr = cJSON_IsArray(got_arr);
    arrsize = cJSON_GetArraySize(got_arr);

    // step 4: Validate (aggregate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(stritem != NULL);
    validation ^= (int)(added_num != 0);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(added_obj != 0);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(isarr != 0);
    validation ^= (int)(arrsize == 2);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    cJSON_free(mem);

    // API sequence test completed successfully
    return 66;
}