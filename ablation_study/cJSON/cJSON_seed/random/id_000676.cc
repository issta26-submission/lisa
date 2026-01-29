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
//<ID> 676
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *retr_arr = (cJSON *)0;
    cJSON *retr_num = (cJSON *)0;
    cJSON *retr_str = (cJSON *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    char *sval = (char *)0;
    int arr_size = 0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(42.0);
    num1 = cJSON_CreateNumber(3.14159);
    str = cJSON_CreateString("example");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, num0);
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddStringToObject(root, "label", "metadata");

    // step 4: Operate
    retr_arr = cJSON_GetObjectItem(root, "data");
    retr_num = cJSON_GetArrayItem(retr_arr, 0);
    val0 = cJSON_GetNumberValue(retr_num);
    retr_num = cJSON_GetArrayItem(retr_arr, 1);
    val1 = cJSON_GetNumberValue(retr_num);
    retr_str = cJSON_GetArrayItem(retr_arr, 2);
    sval = cJSON_GetStringValue(retr_str);
    arr_size = cJSON_GetArraySize(retr_arr);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)added0;
    (void)added1;
    (void)added2;
    (void)val0;
    (void)val1;
    (void)sval;
    (void)arr_size;
    (void)printed;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}