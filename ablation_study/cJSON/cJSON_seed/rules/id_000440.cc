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
//<ID> 440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = NULL;
    cJSON *num_year = NULL;
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_ref = NULL;
    char *printed = NULL;
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_meta = NULL;
    cJSON *parsed_year = NULL;
    cJSON *parsed_list = NULL;
    cJSON *parsed_first = NULL;
    const char *title_val = NULL;
    const char *first_val = NULL;
    double year_val = 0.0;
    int list_size = 0;
    size_t printed_len = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "title", "example"); // add a string to root
    meta = cJSON_AddObjectToObject(root, "meta"); // create nested object under root
    num_year = cJSON_CreateNumber(2026.0); // create number
    cJSON_AddItemToObject(meta, "year", num_year); // add number into nested object
    str_ref = cJSON_CreateStringReference("shared"); // create a string reference
    cJSON_AddItemReferenceToArray(arr, str_ref); // add reference into array
    cJSON_AddItemToObject(root, "list", arr); // attach array to root

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = (printed != NULL) ? (size_t)strlen(printed) : 0;
    parsed = cJSON_ParseWithLengthOpts(printed, printed_len, &parse_end, 1);
    parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    parsed_year = cJSON_GetObjectItem(parsed_meta, "year");
    year_val = cJSON_GetNumberValue(parsed_year);
    title_val = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "title"));
    parsed_list = cJSON_GetObjectItem(parsed, "list");
    list_size = cJSON_GetArraySize(parsed_list);
    parsed_first = cJSON_GetArrayItem(parsed_list, 0);
    first_val = cJSON_GetStringValue(parsed_first);
    buffer[0] = (char)('0' + (list_size % 10));
    buffer[1] = (char)('A' + ((int)year_val % 26));
    buffer[2] = '-';
    buffer[3] = (title_val && title_val[0]) ? title_val[0] : 'X';
    buffer[4] = (first_val && first_val[0]) ? first_val[0] : 'Y';
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}