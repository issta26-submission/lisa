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
//<ID> 976
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "numbers";
    const char *num_key = "count";
    const char *float_key = "floats";
    const char *raw_json = "  { \"x\" : 1 , \"y\": [ 10 , 20 , 30 ] }   ";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *added_number_ptr = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    char *json_buf = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, and a number item then add number to array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    add_ok = cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, arr_key, arr);

    // step 3: Configure - add a numeric field to the object and create+attach a float array
    added_number_ptr = cJSON_AddNumberToObject(root, num_key, 7.0);
    {
        float tmp_floats[3];
        tmp_floats[0] = 1.5f;
        tmp_floats[1] = 2.5f;
        tmp_floats[2] = 3.5f;
        float_arr = cJSON_CreateFloatArray(tmp_floats, 3);
    }
    cJSON_AddItemToObject(root, float_key, float_arr);

    // step 4: Operate - prepare a JSON string buffer, minify it in-place, and compute a validation score
    {
        size_t len = (size_t) (strlen(raw_json) + 1);
        json_buf = (char *)cJSON_malloc(len);
        memcpy(json_buf, raw_json, len);
        cJSON_Minify(json_buf);
    }
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok + (int)(added_number_ptr != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(json_buf != (char *)0 && json_buf[0] != ' ');
    (void)validation_score;

    // step 5: Cleanup - free allocated buffer and delete root (which frees all attached items)
    cJSON_free(json_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}