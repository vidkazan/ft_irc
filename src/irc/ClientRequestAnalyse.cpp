//
// Created by Dmitrii Grigorev on 27.03.22.
//
#include "../main.hpp"

void        Client::analyseRequest()
{
    if(_request.getRequestMethod() == NO_METHOD)
    {
        _response.setRequestIsValid(false);
        _request.setRequestErrors(ERROR_REQUEST_NOT_VALID);
        _request.setReadStatus(REQUEST_READ_COMPLETE);
    }
}