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
//<ID> 2181
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *got = NULL;
    char *printed = NULL;
    void *mem = NULL;
    cJSON_bool added_arr = 0;
    cJSON_bool added_n1 = 0;
    cJSON_bool added_n2 = 0;
    cJSON_bool is_array = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup (create object, array, add numbers, allocate name via cJSON_malloc)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    added_arr = cJSON_AddItemToObject(root, "list", arr);
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    added_n1 = cJSON_AddItemToArray(arr, num1);
    added_n2 = cJSON_AddItemToArray(arr, num2);
    mem = cJSON_malloc(16);
    memset(mem, 0, 16);
    strcpy((char *)mem, "label");
    cJSON_AddStringToObject(root, (const char *)mem, "value");

    // step 3: Core operations (serialize and inspect with required APIs)
    printed = cJSON_PrintUnformatted(root);
    got = cJSON_GetObjectItem(root, "list");
    is_array = cJSON_IsArray(got);
    arr_size = cJSON_GetArraySize(got);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_n1 != 0);
    validation ^= (int)(added_n2 != 0);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got != NULL);
    validation ^= (int)(is_array != 0);
    validation ^= (int)(arr_size == 2);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    cJSON_free(mem);

    // API sequence test completed successfully
    return 66;
}