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
//<ID> 688
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr_direct = (cJSON *)0;
    cJSON *arr_helper = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *s3 = (cJSON *)0;
    char json_buf[64] = "{  \"whitespace\"  :  [  \"a\" ,  \"b\" ]  }";
    char *printed = (char *)0;
    cJSON_bool added_direct = 0;
    int size_direct = 0;
    int size_helper = 0;
    int total_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr_direct = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");

    // step 3: Configure
    cJSON_AddItemToArray(arr_direct, s1);
    cJSON_AddItemToArray(arr_direct, s2);
    added_direct = cJSON_AddItemToObject(root, "direct", arr_direct);
    arr_helper = cJSON_AddArrayToObject(root, "helper");
    s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr_helper, s3);

    // step 4: Operate
    cJSON_Minify(json_buf);
    printed = cJSON_PrintUnformatted(root);
    size_direct = cJSON_GetArraySize(arr_direct);
    size_helper = cJSON_GetArraySize(arr_helper);
    total_size = size_direct + size_helper;

    // step 5: Validate
    (void)added_direct;
    (void)size_direct;
    (void)size_helper;
    (void)total_size;
    (void)json_buf;
    (void)printed;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}